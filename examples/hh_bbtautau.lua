-- Register inputs
local bjet0 = declare_input("bjet0")
local bjet1 = declare_input("bjet1")
local tau0 = declare_input("tau0")
local tau1 = declare_input("tau1")

parameters = {
    energy = 13000.,
    H_mass = 125.09,
    H_width = 0.00407,
}

cuba = {
    relative_accuracy = 0.01,
    verbosity = 3
}

NarrowWidthApproximation.nwa_s12 = {
    mass = parameter('H_mass'),
    width = parameter('H_width')
}

NarrowWidthApproximation.nwa_s34 = {
    mass = parameter('H_mass'),
    width = parameter('H_width')
}

--BreitWignerGenerator.flatter_s12 = {
--    ps_point = add_dimension(),
--    mass = parameter('H_mass'),
--    width = parameter('H_width')
--}

--BreitWignerGenerator.flatter_s34 = {
--    ps_point = add_dimension(),
--    mass = parameter('H_mass'),
--    width = parameter('H_width')
--}

inputs = {bjet0.reco_p4, bjet1.reco_p4, tau0.reco_p4, tau1.reco_p4}

BlockG.blockg = {
    p1 = inputs[1],
    p2 = inputs[2],
    p3 = inputs[3],
    p4 = inputs[4],

--    s12 = 'flatter_s12::s',
--    s34 = 'flatter_s34::s',

    s12 = 'nwa_s12::s',
    s34 = 'nwa_s34::s',
}

Looper.looper = {
    solutions = "blockg::solutions",
    -- path = Path("tf_p1", "tf_p2", "tf_p3", "tf_p4", "initial_state", "hh", "integrand")
    path = Path("tf_p1", "tf_p2", "initial_state", "hh", "integrand")

}

inputs_looper = {'looper::particles/1', 'looper::particles/2', 'looper::particles/3', 'looper::particles/4'}

-- Loop

    GaussianTransferFunctionOnEnergyEvaluator.tf_p1 = {
        reco_particle = inputs[1],
        gen_particle = inputs_looper[1]
    }

    GaussianTransferFunctionOnEnergyEvaluator.tf_p2 = {
        reco_particle = inputs[2],
        gen_particle = inputs_looper[2]
    }

    -- GaussianTransferFunctionOnEnergyEvaluator.tf_p3 = {
    --     reco_particle = inputs[3],
    --     gen_particle = inputs_looper[3]
    -- }

    -- GaussianTransferFunctionOnEnergyEvaluator.tf_p4 = {
    --     reco_particle = inputs[4],
    --     gen_particle = inputs_looper[4]
    -- }

    BuildInitialState.initial_state = {
        particles = inputs_looper
    }

    -- jacobians = {'nwa_s12::jacobian', 'nwa_s34::jacobian',  'tf_p1::TF', 'tf_p2::TF', 'tf_p3::TF', 'tf_p4::TF', 'looper::jacobian'}
    jacobians = {'nwa_s12::jacobian', 'nwa_s34::jacobian',  'tf_p1::TF', 'tf_p2::TF', 'looper::jacobian'}

--    jacobians = {'flatter_s12::jacobian', 'flatter_s34::jacobian',  'tf_p1::TF', 'tf_p2::TF', 'tf_p3::TF', 'tf_p4::TF', 'looper::jacobian'}

    MatrixElement.hh = {
        pdf = 'CT10nlo',
        pdf_scale = parameter('H_mass'),

        matrix_element = 'hh_bbtautau_ME_SMEFT_FF_2_P1_Sigma_SMEFT_FF_2_gg_bbxtaptam',
        matrix_element_parameters = {
            card = '../MatrixElements/hh_bbtautau_ME/Cards/param_card.dat'
        },

        initialState = 'initial_state::partons',

        particles = {
          inputs = inputs_looper,
          ids = {
            {
              pdg_id = 5,
              me_index = 1,
            },
            {
              pdg_id = -5,
              me_index = 2,
            },
            {
              pdg_id = -15,
              me_index = 3,
            },
            {
              pdg_id = 15,
              me_index = 4,
            },
          }
        },

        jacobians = jacobians
    }

    DoubleLooperSummer.integrand = {
        input = "hh::output"
    }

-- End of loop

integrand("integrand::sum")
